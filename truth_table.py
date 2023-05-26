from itertools import product


class row:
    def __init__(self, bits: list[int]):
        assert all(bit in [0, 1] for bit in bits)
        self.bits = bits

    def __str__(self):
        string = "["
        for b in self.bits[:-1]:
            string += str(b) + ", "
        string += str(self.bits[-1])
        string += "]"
        return string

    def __getitem__(self, bit_index):
        assert bit_index in range(len(self))
        return self.bits[bit_index]

    def __setitem__(self, bit_index, value):
        assert bit_index in range(len(self))
        assert value in [0, 1]
        self.bits[bit_index] = value

    def __len__(self):
        return len(self.bits)

    def negate(self, bit_index: int | None = None):
        assert bit_index in range(len(self)) or bit_index is None
        if bit_index is None:
            for bit_index, _ in enumerate(self.bits):
                self.negate(bit_index)
        else:
            self.bits[bit_index] = self.bits[bit_index] * (-1) + 1


class truth_table:
    def __init__(self, size=0):
        base_bits = [0, 1]
        tt_bits = product(base_bits, repeat=size)

        self.table = []
        for row_bits in tt_bits:
            row_bits = list(row_bits)
            row_bits.reverse()
            self.table.append(row(row_bits))

    def __str__(self):
        string = ""
        for row in self.table[:-1]:
            string += str(row)
            string += "\n"
        string += str(self.table[-1])
        return string

    def __len__(self):
        return len(self.table)

    def __getitem__(self, row_index: int):
        assert row_index in range(len(self))
        return self.table[row_index]

    def __setitem__(self, row_index: int, row_value: row):
        assert row_index in range(len(self))
        assert len(row_value) == len(self.table[row_index])
        self.table[row_index] = row_value

    def set_to_table(self, table: list[list[int]]):
        size = len(table[0])
        assert all(len(table_row) == size for table_row in table)
        assert len(table) == size**2
        self.table = [row(table_row) for table_row in table]

    def negate_row(self, row_index: int):
        assert row_index in range(len(self.table))
        self[row_index].negate()

    def negate_column(self, column_index: int):
        for row in self.table:
            row.negate(column_index)


tt = truth_table(4)
tt[0] = row([1, 1, 1, 1])
print(tt, end="\n\n")
print(len(tt))
tt.negate_row(len(tt) - 1)
print(tt, end="\n\n")
