from lxml import etree


def main():
    xmlDoc = etree.parse("crca.1.0.xml")
    root = xmlDoc.getroot()

    for node in xmlDoc.iter():
        print("path: ", xmlDoc.getpath(node))

if __name__ == "__main__":
    main()