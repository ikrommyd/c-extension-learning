from setuptools import setup, Extension


setup(
    name="custom3",
    version="0.1.0",
    ext_modules=[Extension("custom3", ["custom3.c"])]
)

