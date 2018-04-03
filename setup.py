from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import hashlib
import platform
import subprocess
import tensorflow as tf
from setuptools import setup, Extension

__VERSION__ = '1.4.2'  # update version in __init__.py too


def pkg_config(*packages):
    return subprocess.check_output([
        'pkg-config',
        '--libs',
        '--cflags',
        ' '.join(packages)
    ]).decode('utf8').split()


def tf_config():
    return tf.sysconfig.get_compile_flags() + tf.sysconfig.get_link_flags()


def ops_config():
    cpp_flags = ['-std=c++11', '-shared', '-fPIC', '-O2']
    os_flags = ['-undefined dynamic_lookup'] if platform.system() == 'Darwin' else []

    return cpp_flags + os_flags


def parse_args(flags, **kwargs):
    flag_map = {
        '-D': 'define_macros',
        '-I': 'include_dirs',
        '-L': 'library_dirs',
        '-l': 'libraries'
    }
    for flag in flags:
        if flag[:2] in flag_map:
            kwargs.setdefault(flag_map.get(flag[:2]), []).append(flag[2:])
        else:
            kwargs.setdefault('extra_compile_args', []).append(flag)

    kwargs.setdefault('define_macros', [])
    kwargs['define_macros'] = [tuple(m.split('=')) for m in kwargs['define_macros']]

    return kwargs


def flags_hash(flags):
    key = '/'.join(flags)
    key = key.encode('utf-8')

    return hashlib.md5(key).hexdigest()


setup(
    name='tfucops',
    version=__VERSION__,
    description='Unicode string OPs for TensorFlow',
    url='https://github.com/shkarupa-alex/tfucops',
    author='Shkarupa Alex',
    author_email='shkarupa.alex@gmail.com',
    license='MIT',
    keywords='tensorflow unicode string op',
    packages=['tfucops'],
    include_package_data=True,
    install_requires=[
        'tensorflow>=1.5.0'
    ],
    ext_modules=[
        Extension(
            'tfucops_{}'.format(flags_hash(tf_config() + [__VERSION__])),  # bind ext to particular TF instance
            sources=['source/tfucops.cpp'],
            **parse_args(ops_config() + tf_config() + pkg_config('icu-uc'))
        )
    ],
    test_suite='nose.collector',
    tests_require=['nose']
)
