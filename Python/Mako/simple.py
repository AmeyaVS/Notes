from mako.template import Template

tmp = Template("hello ${name}")

print(tmp.render(name="world!"))