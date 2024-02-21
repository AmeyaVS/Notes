from mako.template import Template
from mako.lookup import TemplateLookup

def main():
    tmpllookup = TemplateLookup(directories=["templates", "templates/html"])
    mytmpl = tmpllookup.get_template("base.html.mako")
    print(mytmpl.render())

if __name__ == "__main__":
    main()