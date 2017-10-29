# Python tips and tricks.

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
