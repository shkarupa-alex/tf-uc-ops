from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from setuptools import setup
from setuptools.dist import Distribution

class BinaryDistribution(Distribution):
    def has_ext_modules(foo):
        return True

setup(
    name='tfucops',
    version='0.1',
    description='Unicode string OPs for TensorFlow',
    url='https://github.com/shkarupa-alex/tfucops',
    author='Shkarupa Alex',
    author_email='shkarupa.alex@gmail.com',
    license='MIT',
    keywords='tensorflow unicode string op',
    packages=['tfucops'],
    install_requires=[
        'tensorflow>=1.5.0'
    ],
    package_data={
        '': ['*.so'],
    },
    distclass=BinaryDistribution,
)
