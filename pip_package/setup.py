from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from setuptools import setup, find_packages
from setuptools.dist import Distribution

__VERSION__ = '2.0.0'


class BinaryDistribution(Distribution):
    """This class is needed in order to create OS specific wheels."""

    def has_ext_modules(self):
        return True


print('tfunicode will fail when building from a source distribution (sdist). Please follow instructions in '
      '(https://github.com/shkarupa-alex/tfunicode/README.md) to build this from the source.')

with open('README.md', 'r') as fh:
    long_description = fh.read()

setup(
    name='tfunicode',
    version=__VERSION__,
    description='Unicode string ops for TensorFlow',
    long_description=long_description,
    long_description_content_type="text/markdown",
    url='https://github.com/shkarupa-alex/tfunicode',
    author='Shkarupa Alex',
    author_email='shkarupa.alex@gmail.com',
    license='MIT',
    keywords='tensorflow unicode string op',
    packages=find_packages(),
    install_requires=[
        'tensorflow>=1.8.0',
    ],
    include_package_data=True,
    package_data={'': ['*.so']},
    exclude_package_data={'': ['BUILD', '*.h', '*.cc']},
    zip_safe=False,
    distclass=BinaryDistribution,
    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Education',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'Topic :: Scientific/Engineering',
        'Topic :: Scientific/Engineering :: Artificial Intelligence',
        'Topic :: Software Development',
        'Topic :: Software Development :: Libraries',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ],
)
