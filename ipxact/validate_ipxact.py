from lxml import etree
import xmlschema
import urllib
import os.path

def main():
    tree = etree.parse("crca.1.0.xml")

    # Get root element
    root = tree.getroot()

    # Print namespaces
    print(f"Document Namespaces: {root.nsmap}")

    root_ns_uri= tree.xpath("namespace-uri()")

    #tree.xpath("namespace-uri()", namespaces=tree.getroot().nsmap)

    print(f"Root Namespace: {root_ns_uri}")

    root =  tree.getroot()

    print(root.tag)
    print(root.attrib)

    # Get Schema Namespace
    base_schema_ns = f"{{{root.nsmap['xsi']}}}schemaLocation"
    print(base_schema_ns)

    if base_schema_ns in root.attrib:
        l = root.attrib[base_schema_ns].split()
        if root_ns_uri in l:
            schema_base_uri = l[1]

    print(schema_base_uri)

    # Extract Path from URI
    path = urllib.parse.urlparse(schema_base_uri).path
    print(path)

    schema_path = os.path.split(path)

    print(schema_path[0])

    target_path = f"schemas/{schema_path[0]}"

    # Create Directory if it doesn't exists:
    os.makedirs(target_path, exist_ok=True)

    # Get XML Schema only once per execution
    if not os.path.isfile(f"{target_path}/{schema_path[1]}"):
        schema = xmlschema.XMLSchema(schema_base_uri)
        # Currently disabled
        schema.export(target=target_path, save_remote=True)
    else:
        schema = xmlschema.XMLSchema(f"{target_path}/{schema_path[1]}")

    # root_ns = f"{{{root_ns_uri}}}component"

    # print(root_ns)

    # print(root.attrib["schemaLocation"])

    # for elem in root.findall(f"./"):
    #    #print(f"{tree.xpath("/ipxact:component")}")
    #    print(f"elem: {elem.tag} {elem.attrib}")

    # for action, elem in etree.iterwalk(root):
    # for elem in root.iter():
    #     #print(f"{elem.tag} {elem.attrib}")
    #     #print(type(elem.attrib))
    #     #if ("f{elem.attrib}" != "{}"): # or (elem.attrib != {}):
    #     #    #print(f"{action}: {elem.tag}")
    #     #    print(f"{elem.tag}: {elem.attrib}")
    #     val = elem.attrib.get(f"{{{root.nsmap['xsi']}}}schemaLocation")
    #     #print(f"{{{root.nsmap["xsi"]}}}schemaLocation")
    #     if val != None: # and f"{{{root.nsmap["xsi"]}}}schemaLocation" in val:
    #         print(f"{elem.tag}: {elem.attrib}")
    #         #print(f"{elem.getelementpath()}")

# schema = xmlschema.XMLSchema("https://www.omg.org/spec/ReqIF/20110401/reqif.xsd")
    
if __name__ == "__main__":
    main()