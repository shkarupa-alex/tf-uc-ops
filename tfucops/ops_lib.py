from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import hashlib
import os
import platform
import pkg_resources
import subprocess
import tensorflow as tf


try:
    __version__ = pkg_resources.get_distribution("construct").version
except:
    __version__ = 'dev'


_ops_lib = None # global variable to cache loaded library


def _ops_lib_on_demand():
    global _ops_lib
    if _ops_lib is not None:
        return _ops_lib

    # get compilation flags from installed TF
    tf_cflags = tf.sysconfig.get_compile_flags()
    tf_lflags = tf.sysconfig.get_link_flags()

    # make hash to identify correct ops build
    build_refs = [tf.__version__, __version__] + \
                 tf_cflags + tf_lflags + \
                 [platform.platform(), platform.python_version()]
    build_key = u'_'.join(build_refs)
    build_hash = hashlib.sha224(build_key.encode('utf-8')).hexdigest()

    # get absolute paths to target
    base_dir = os.path.dirname(os.path.abspath(__file__))
    build_target = os.path.join(base_dir, 'tfucops.so.' + build_hash)

    # build on-demand
    if not os.path.exists(build_target):
        tf.logging.debug(u'ops library not found, building...')

        # get ICU compilation flags
        icu_cmd = ['pkg-config', '--libs', '--cflags', 'icu-uc']
        tf.logging.debug(u'running {}'.format(u' '.join(icu_cmd)))
        pkg_config = subprocess.Popen(icu_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = pkg_config.communicate()
        if err:
            raise Exception('pkg-config command failed with message: {}'.format(err))
        if pkg_config.returncode != 0:
            raise Exception('pkg-config command failed with code {}'.format(pkg_config.returncode))
        icu_flags = out.decode('utf-8').split()

        # evaluate OS specific flags
        os_flags = ['-undefined', 'dynamic_lookup'] if 'Darwin' == platform.system() else []

        # get absolute paths to source
        build_source = os.path.join(base_dir, 'src', 'tfucops.cc')

        # remove target from previous build
        tf.logging.debug(u'removing old build target')
        try:
            os.remove(build_target)
        except OSError:
            pass

        # build shared OPs library
        build_cmd = ['g++', '-std=c++11', '-shared', '-o', build_target, '-fPIC'] \
                    + tf_cflags + tf_lflags + os_flags + icu_flags + ['-O2', build_source]
        print(build_cmd)
        tf.logging.debug(u'running {}'.format(u' '.join(build_cmd)))
        gnu_compiller = subprocess.Popen(build_cmd)
        out, err = gnu_compiller.communicate()
        if err:
            raise Exception('g++ command failed with message: {}'.format(err))
        if gnu_compiller.returncode != 0:
            raise Exception('g++ command failed with code {}'.format(gnu_compiller.returncode))

        _ops_lib = tf.load_op_library(build_target)

    return _ops_lib
