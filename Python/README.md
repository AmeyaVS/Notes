# Python tips and tricks #

## Python Development ##

* Use `virtualenv` for development of isolated environment.

    1. You should have a basic working pip tool setup available.(Python2: pip2/ Python3: pip3)  
    **Use the distributions package manager to install the pip tool.**
    2. Follow the step to install `virtualenv`:
        ```sh
        # Use pip2 or pip3 according to the availabilty.
        # Referenced here as `pip`
        pip install --user virtualenv
        ```
        **Note:** You should have `$HOME/.local/bin` in your `$PATH` environment variable.

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
