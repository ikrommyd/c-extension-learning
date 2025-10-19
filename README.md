# Learning to write C extensions for Python
Collecting stuff while learning to write C extensions for Python.

### Setup

You need a build of python installed. You can get one using `pyenv` or build it from source.
Python is included as a submodule so you can build the debug build from that or you can use any clone of CPython that you want.
To compile python from source, I typically do the following:
```bash
pushd submodules/cpython
PYTHON_VERSION="3.13.8"
PYTHON_MINOR=$(echo $PYTHON_VERSION | grep -oE '^[0-9]+\.[0-9]+')

git checkout "v$PYTHON_VERSION"
./configure --prefix=/opt/python/${PYTHON_VERSION}/ --with-pydebug
sudo make -j 12
sudo make altinstall

sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/python${PYTHON_MINOR}        /opt/python/${PYTHON_VERSION}/bin/python3
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/python${PYTHON_MINOR}        /opt/python/${PYTHON_VERSION}/bin/python
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/pip${PYTHON_MINOR}           /opt/python/${PYTHON_VERSION}/bin/pip3
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/pip${PYTHON_MINOR}           /opt/python/${PYTHON_VERSION}/bin/pip
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/pydoc${PYTHON_MINOR}         /opt/python/${PYTHON_VERSION}/bin/pydoc
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/idle${PYTHON_MINOR}          /opt/python/${PYTHON_VERSION}/bin/idle
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/python${PYTHON_MINOR}-config /opt/python/${PYTHON_VERSION}/bin/python-config

sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/python${PYTHON_MINOR}        /opt/local/bin/python${PYTHON_MINOR}
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/pip${PYTHON_MINOR}           /opt/local/bin/pip${PYTHON_MINOR}
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/pydoc${PYTHON_MINOR}         /opt/local/bin/pydoc${PYTHON_MINOR}
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/idle${PYTHON_MINOR}          /opt/local/bin/idle${PYTHON_MINOR}
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/python${PYTHON_MINOR}d       /opt/local/bin/python${PYTHON_MINOR}d
sudo ln -sf /opt/python/${PYTHON_VERSION}/bin/python${PYTHON_MINOR}d-config /opt/local/bin/python${PYTHON_MINOR}d-config

popd
```

Then I make a virtual environment with `uv` that is pointed to the debug build of python:
```bash
uv venv --python /opt/python/3.13.8/bin/python3
source .venv/bin/activate
uv pip install -r requirements.txt
```
