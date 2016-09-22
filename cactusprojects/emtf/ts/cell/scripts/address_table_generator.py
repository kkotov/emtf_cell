#!/usr/bin/env python

from __future__ import print_function
import sys
import csv


class CSVLineHandler():
    def __init__(self):
        # this needs to be ordered dict but they are introduced in python 2.7 and here we have 2.6
        self.fields_ordered = [('register',    13),
                               ('accessmode',  'memory'),
                               ('bar',         '0'),
                               ('address',     4),
                               ('mask',        6),
                               ('read',        7),
                               ('write',       8),
                               ('description', 9)]

        self.fields = {}
        for field in self.fields_ordered:
            self.fields[field[0]] = field[1]

        self.csv_fields_size = {}
        for key in self.fields.keys():
            if isinstance(self.fields[key], int):
                self.csv_fields_size[key] = 0

        self.delimiter = 8

    def update_csv_field_size(self, line):
        for key in self.csv_fields_size.keys():
            if self.csv_fields_size[key] < len(line[self.fields[key]]):
                self.csv_fields_size[key] = len(line[self.fields[key]])

    def extract_csv_line(self, line):
        extracted_line = ''

        if line[self.fields['register']]:
            for field in self.fields_ordered:
                key = field[0]
                if isinstance(self.fields[key], int):
                    extracted_line += line[self.fields[key]].ljust(self.csv_fields_size[key])
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
                csv_line_handler.update_csv_field_size(line)

                register = csv_line_handler.register_name(line)
                if register:
                    if register not in registers:
                        registers.add(register)
                    else:
                        print('The register name "{0}" is duplicated'.format(register))

            f_in.seek(0)
            f_in.next()
            for line in reader:
                extracted_line = csv_line_handler.extract_csv_line(line)
                if extracted_line:
                    f_out.write('{0}\n'.format(extracted_line))

# TODO: add a function that creates the header lines in the addres table

