from setuptools import setup, Extension


setup(
    name="custom2",
    version="0.1.0",
    description="Python interface for the fputs C library function",
    ext_modules=[Extension("custom2", ["custom2.c"])]
)

