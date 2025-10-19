from setuptools import setup, find_packages, Extension


setup(
    name='queue',
    version='0.1.0',
    packages=find_packages(),
    license='GPL-2',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Natural Language :: English',
        'Programming Language :: Python :: 3 :: Only',
        'Programming Language :: Python :: Implementation :: CPython',
    ],
    ext_modules=[
        Extension(
            # the qualified name of the extension module to build
            'myqueue',
            # the files to compile into our module relative to ``setup.py``
            ['queue.c'],
        ),
    ],
)
