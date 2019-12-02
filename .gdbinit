python

import gdb.printing

class VecPrinter:
    def __init__(self, v):
        self.v = v

    def to_string(self):
        return "x = {}, y = {}, z = {}\n".format(self.v['x'], self.v['y'], self.v['z'])


def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("GLM_pp")
    pp.add_printer('glm::vec', 'vec', VecPrinter)
    return pp

gdb.printing.register_pretty_printer(gdb.current_objfile(),
                                     build_pretty_printer())

end

winheight src -10

define hook-stop
	refresh
end

define fs
	focus src
end

define fc
	focus cmd
end