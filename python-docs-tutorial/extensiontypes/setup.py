from setuptools import setup, Extension


setup(
    name="custom",
    version="0.1.0",
    ext_modules=[
        Extension("custom", ["custom.c"]),
        Extension("custom2", ["custom2.c"]),
        Extension("custom3", ["custom3.c"]),
        Extension("custom4", ["custom4.c"]),
    ]
)
