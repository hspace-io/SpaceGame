from setuptools import setup, Extension
from Cython.Build import cythonize
import os


extensions = [
    Extension(
        "nunpy",
        sources=["build/tmp.pyx", "build/nunpy.cpp"],
        language="c++",
        extra_compile_args=["/std:c++17"] if os.name == "nt" else ["-std=c++17"]
    )
]

setup(
    name="nunpy",
    ext_modules=cythonize(extensions, build_dir="."),
)
