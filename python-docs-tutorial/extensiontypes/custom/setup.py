from setuptools import setup, Extension


setup(
    name="custom",
    version="0.1.0",
    ext_modules=[Extension("custom", ["custom.c"])]
)

