from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from setuptools import setup

setup(
    name='tfucops',
    version='1.0.6',
    description='Unicode string OPs for TensorFlow',
    url='https://github.com/shkarupa-alex/tfucops',
    author='Shkarupa Alex',
    author_email='shkarupa.alex@gmail.com',
    license='MIT',
    keywords='tensorflow unicode string op',
    packages=['tfucops'],
    package_data={
        '': ['*.h', '*.cc'],
    },
    install_requires=[
        'tensorflow>=1.5.0'
    ],
)
