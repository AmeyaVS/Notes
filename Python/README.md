# Python tips and tricks #

## Python Development ##

* Use `virtualenv` and `virtualenvwrapper` for development of isolated environment.

    1. You should have a basic working pip tool setup available.(Python2: pip2/ Python3: pip3)  
       **Use the distributions package manager to install the pip tool.**
    2. Follow the step to install `virtualenv` and `virtualenvwrapper`:
        ```sh
        # Use pip2 or pip3 according to the availabilty.
        # Referenced here as `pip`
        pip install --user virtualenv virtualenvwrapper
        ```
        **Note:** You should have `$HOME/.local/bin` in your `$PATH` environment variable.
    3. Setting up a virtualenvwrapper environment:
        ```sh
        # Setup following lines in your ~/.bashrc
        export WORKON_HOME=$HOME/apps/pyenv
        # Locate Python3 executable using this command: $ which python3
        export VIRTUALENVWRAPPER_PYTHON=/usr/bin/python3
        source ~/.local/bin/virtualenvwrapper.sh
        ```
    4. Creating a virtualenvironment:
        ```sh
        # For default Python 3 Environment
        mkvirtualenv py3grip
        # Check Virtualenv Python version
        python --version
        pip install grip
        # For Python 2 Environment
        mkvirutalenv py2grip -p python2
        # Check Virtualenv Python version
        python --version
        pip install grip
        ```
    5. Activating the environment:
        ```sh
        workon py3grip
        ```
    6. De-activating the virtual environment:
        ```sh
        deactivate
        ```
**NOTE:** Refer the documentation of virtualenvwrapper for user-defined hooks
for setting up a development environment for C/C++ Projects.

---

**Old steps**


    3. Setting up a virtual environment for development:  
       **Only one python executable can be used for the virtual environment setup.**
        ```sh
        # For Python2 Development
        virtualenv -p /usr/bin/python2.7 my_project
        # For Python3 Development
        virtualenv -p /usr/bin/python3 my_project
        ```
    4. Activating the virtual environment:  
       For Linux:
        ```sh
        source my_project/bin/activate
        ```
       For Windows:
        ```bat
        .\my_project\Scripts\activate
        ```
    5. De-activating the virtual environment:
        ```sh
        deactivate
        ```
---

## Utilities ##

* Simple and easy accessible __*webserver*__ for quick file sharing.

    ```sh
    # For python 2.x
    python -m SimpleHTTPServer <custom_port_number>

    # For Python 3.x
    python3 -m http.server
    ```
* Simple Python `HTTPS Server`.

    ```python
    #!/usr/bin/python
    # taken from http://www.piware.de/2011/01/creating-an-https-server-in-python/
    # generate server.xml with the following command:
    #    openssl req -new -x509 -keyout server.pem -out server.pem -days 365 -nodes
    # run as follows:
    #    python simple-https-server.py
    # then in your browser, visit:
    #    https://localhost:4443

    import BaseHTTPServer, SimpleHTTPServer
    import ssl

    httpd = BaseHTTPServer.HTTPServer(('localhost', 4443),
    SimpleHTTPServer.SimpleHTTPRequestHandler)
    httpd.socket = ssl.wrap_socket (httpd.socket, certfile='./server.pem',
    server_side=True)
    httpd.serve_forever()
    ```
    **Note:** For key and certificate generation kindly search online for better generation
    process.

**Notes:**

1. By default port number is: `8000`.
2. The `HTTP Server` is **unsecured!!!**, anybody with network access can access
   the directory from where the script is started.
3. Kindly use it only on **Private and Trusted Network**.

## Tools and Libraries ##

List of Python libraries and tools:

**NOTE:** Use the virtualenvwrapper to create separate virtual environment for
these libraries/tools as they might have conflicting dependencies.

**Available as command line**

```sh
pip install --user \
                   flask \
                   gdbgui \
                   grip \
                   jupyter \
                   matplotlib \
                   numpy \
                   scipy
```

## To-Do ##

1. Securing the Jupyter NoteBook installation.
2. Add Code for QRCode Generator.
3. Add the Python API Tracer.
4. ......

## CTypes Example

* Ctypes library for calling `C/C++` API's from python.
* Simple example available in [ctypes](ctypes) directory.
* [flask_tutorial](flask_tutorial): An exploration of Flask Tool.
* [Jinja2](Jinja2): An exploration of Templating Engine can be used along with Flask
* [Mako](Mako): An exploration of another Templating Engine.

