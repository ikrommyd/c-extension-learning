from setuptools import setup, Extension


setup(
    name="buildvalue",
    version="0.1.0",
    description="Python interface for the fputs C library function",
    ext_modules=[Extension("buildvalue", ["buildvalue.c"])]
)

