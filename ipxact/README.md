# IP-XACT

This directory contains simple scripts to Parse and Validate IP-XACT documents.

For IP-XACT reference to the standard visit this [site](https://accellera.org/downloads/standards/ip-xact) for more details.

## Pre-requisites

Install following python packages in your Python Virtual Environment.

* [lxml](https://lxml.de/), [lxml source](https://github.com/lxml/lxml): For Generic Parsing of IP-XACT XML Files.
* [xmlschema](https://github.com/sissaschool/xmlschema): For Validating the Input against the IP-XACT XML XSD Schema definitions.

## Sample Scripts

Here's a collection of sample scripts:

* [list_xpaths.py](list_xpaths.py): Lists the XPATHs to the elements in the IP-XACT specifications.
* [validate_ipxact.py](validate_ipxact.py): Validate an IP-XACT Document against the IP-XACT XSD Schema from the standard.
* [get_ipxact2022_xsd.py](get_ipxact2022_xsd.py): Get XML Schemas for IP-XACT 2022 release.
