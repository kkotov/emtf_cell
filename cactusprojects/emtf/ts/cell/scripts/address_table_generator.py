#!/usr/bin/env python

from __future__ import print_function
from collections import OrderedDict
import sys
import csv


class CSVLineHandler():
    ''' Processes a single line of the input .cvs file.

    The class provides functionality that:
        - checks if the regiters has unique names (no duplicates are allowed)
        - extracts the needed fields from the input .cvs file (line by line)
        - aligns the extracted data in columns so it's easy readable
        - produces a formatted string from the extracted data and outputs it to the .txt file
    '''

    def __init__(self):
        '''

        self.fields
            - holds the name and the position of all the fields needed from  the input .csv file
            - the order of the items is important, because that will be the order in the output .txt file
            - some of the fields like 'accessmode' and 'bar' are not even in the input .csv file so their values are hardcoded

        self.csv_fields_size
            - holds the length of the column for the corresponding field in the output .txt file
            - without this info the output .txt file will not be aligned nicely in columns
        '''

        self.fields = OrderedDict([('register',    13),
                                   ('accessmode',  'memory'),
                                   ('bar',         '0'),
                                   ('address',     4),
                                   ('mask',        6),
                                   ('read',        7),
                                   ('write',       8),
                                   ('description', 9)])

        self.csv_fields_size = {}
        for key in self.fields.keys():
            if isinstance(self.fields[key], int):
                self.csv_fields_size[key] = 0

        self.delimiter = 8

    def update_csv_field_size(self, line):
        ''' Build the info about the length of the output columns beased on the content of the input .csv file. '''

        for key in self.csv_fields_size.keys():
            if self.csv_fields_size[key] < len(line[self.fields[key]]):
                self.csv_fields_size[key] = len(line[self.fields[key]])

    def extract_csv_line(self, line):
        ''' Extracts all the needed fields from a given line and forms the string that will be written to the output .txt file.'''

        extracted_line = ''

        if line[self.fields['register']]: # If the register_name is missing skip the line.
            for key in self.fields.keys():
                if isinstance(self.fields[key], int):
                    extracted_line += line[self.fields[key]].ljust(self.csv_fields_size[key])
                else:
                    if (key == 'bar') and \
                                          ((line[self.fields['register']].find('prim_conv_') == 0) or
                                           (line[self.fields['register']] == 'ptlut_mem') or
                                           (line[self.fields['register']] == 'ptlut_addr')):
                        # TODO: This 'if' is a hack and it needs to be fixed!
                        # The reason for the hack is that we don't know exactly how to determine the value of bar.
                        extracted_line += '1'
                    else:
                        extracted_line += self.fields[key]

                extracted_line += ' ' * self.delimiter

        return extracted_line.strip()

    def register_name(self, line):
        return line[self.fields['register']]


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('usage: {0} file_name.csv'.format(sys.argv[0]))
        exit(1)

    input_file = sys.argv[1]
    csv_line_handler = CSVLineHandler()

    output_file = input_file[0:-4] + '.txt'

    with open(input_file, 'r') as f_in:
        with open(output_file, 'w') as f_out:
            reader = csv.reader(f_in)

            registers = set()
            for line in reader:
                # Extract inpormation about the length of the output fields.
                # This is important, because otherwise the output file won't be formatted properly and won't be human readable.
                csv_line_handler.update_csv_field_size(line)

                # Check if all the register_names are unique, because this is mandatory. If duplicates are found inform the user.
                register = csv_line_handler.register_name(line)
                if register: # skip the lines that don't have register_name
                    if register not in registers:
                        registers.add(register)
                    else:
                        print('ERROR: the register name "{0}" is duplicated'.format(register))

            # When the field sizes are known and the user is notified about duplicate register names (if any) continue with the real data processing.
            f_in.seek(0)
            f_in.next()
            for line in reader:
                extracted_line = csv_line_handler.extract_csv_line(line)
                if extracted_line:
                    f_out.write('{0}\n'.format(extracted_line))

