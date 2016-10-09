import unittest
import sys
import re

import syscalltab

SYSCALL_EXPAND_H='_syscall_expand.h'

class TestSysCallTabMethods(unittest.TestCase):

    def setUp(self):
        self.sc_map = {}
        sc_content = re.compile('^#define\s__NR_(\w+)\s([0-9]+)$')

        with open(SYSCALL_EXPAND_H, 'r') as sc_expand:
            for expanded_header_line in sc_expand:
                sc_match = sc_content.match(expanded_header_line)

                if sc_match:
                    self.sc_map[sc_match.group(1)] = int(sc_match.group(2))
                    self.sc_map[int(sc_match.group(2))] = sc_match.group(1)

        self.sc_count = (len(self.sc_map) / 2)

    def test_count_value_is_nonzero(self):
        self.assertTrue(syscalltab.count() != 0)

    def test_count_value_equal_with_cpp_count(self):
        self.assertTrue(syscalltab.count() == self.sc_count)

    def _gen_function_syscalls_so(self, syscallname):
        def generic(*args):
            so_value = syscalltab.number(syscallname)
            sys.stderr.write("asserting {0} in .so *** ".format(syscallname))
            self.assertTrue(so_value is not None)

        generic.__name__ = "is_{0}_syscall_in_so".format(syscallname)
        return generic

    def _gen_function_syscall_so_number_equal_with_cpp(self, syscallname):
        def generic(*args):
            so_value = syscalltab.number(syscallname)
            cpp_value = int(self.sc_map[syscallname])

            sys.stderr.write("asserting {0} .so value {1} == cpp value {2}\n".format(syscallname, so_value, cpp_value))
            self.assertEqual(so_value, cpp_value)

        generic.__name__ = "is_{0}_syscall_equal_with_cpp".format(syscallname)
        return generic

    def _gen_function_syscall_name_so_equal_with_cpp(self, syscallnumber):
        def generic(*args):
            so_name = syscalltab.name(syscallnumber)
            cpp_name = str(self.sc_map[syscallnumber])

            sys.stderr.write("asserting {0} .so name {1} == cpp name {2}\n".format(syscallnumber, so_name, cpp_name))
            self.assertEqual(so_name, cpp_name)
            
        generic.__name__ = "is_{0}_syscall_name_equal_with_cpp_name".format(syscallnumber)
        return generic

    def test_syscalls_in_so(self):
        for syscall in self.sc_map.keys():
            if isinstance(syscall, basestring) is True:
                testfn = self._gen_function_syscalls_so(syscall)
                testfn(syscall)

    def test_syscalls_so_equal_with_cpp(self):
        for syscall in self.sc_map.keys():
            testfn = None

            if isinstance(syscall, int) is True:
                testfn = self._gen_function_syscall_name_so_equal_with_cpp(syscall)
            elif isinstance(syscall, basestring) is True:
                testfn = self._gen_function_syscall_so_number_equal_with_cpp(syscall)

            testfn(syscall)


if __name__ == '__main__':
    tests = unittest.TestLoader().loadTestsFromTestCase(TestSysCallTabMethods)
    unittest.TextTestRunner(verbosity=2).run(tests)
