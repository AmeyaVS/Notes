import xmlschema
import os.path
import urllib

def main():
    xsd_base_uri = "http://www.accellera.org/XMLSchema/IPXACT/1685-2022/index.xsd"

    # Extract Path from URI
    path = urllib.parse.urlparse(xsd_base_uri).path
    print(path)

    # Split path into path + resource name
    schema_path = os.path.split(path)
    print(schema_path)

    target_path = f"schemas/{schema_path[0]}"

    # Create Directory if it doesn't exists:
    os.makedirs(target_path, exist_ok=True)

    local_target_path = f"{target_path}/{schema_path[1]}"

    if not os.path.isfile(f"{local_target_path}"):
        schema = xmlschema.XMLSchema(xsd_base_uri)
        schema.export(target=target_path, save_remote=True, remove_residuals=True)
    schema  = xmlschema.XMLSchema(local_target_path)
    
    


if __name__ == '__main__':
    main()