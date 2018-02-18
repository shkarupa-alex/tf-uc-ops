from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from setuptools import setup

setup(name='tf-icu-ops',
      version='0.1',
      description='Unicode string OPs for TensorFlow',
      url='https://github.com/shkarupa-alex/tf-uc-ops',
      author='Shkarupa Alex',
      author_email='shkarupa.alex@gmail.com',
      license='MIT',
      packages=['tf-uc-ops'],
      install_requires=[
          'tensorflow'
      ],
      zip_safe=False,
      test_suite='nose.collector',
      tests_require=['nose'])
