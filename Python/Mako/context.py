from mako.template import Template
from mako.runtime import Context
from io import StringIO

mytemplate = Template("hello, ${name}!")
buf = StringIO()
ctx = Context(buf, name="jack")
mytemplate.render_context(ctx)
print(buf.getvalue())