from itertools import product


class row:
    def __init__(self, bits: list[int]):
        assert all(bit in [0, 1] for bit in bits)
        self.bits = bits

    def __str__(self):
        string = "["
        for b in self.bits[:-1]:
            string += str(b) + ","
        string += str(self.bits[-1])
        string += "]"
        return string

    def size(self):
        return len(self.bits)


class truth_table:
    def __init__(self, size=1):
        base_bits = [0, 1]
        tt_bits = product(base_bits, repeat=size)

        self.table = []
        for row_bits in tt_bits:
            row_bits = list(row_bits)
            row_bits.reverse()
            self.table.append(row_bits)

    def __str__(self):
        string = ""
        for r in self.table[:-1]:
            string += str(r)
            string += "\n"
        string += str(self.table[-1])
        return string

    def set_to_table(self, table):
        size = len(table[0])
        assert all(len(table_row) == size for table_row in table)
        self.table = [row(table_row) for table_row in table]


tt = truth_table(4)
tt.set_to_table(
    [
        [0, 1],
        [1, 1],
        [1, 0],
        [0, 0],
    ]
)
print(tt)

# class gate:
#     def __init__(self, size, controls: list[int], target: int):
#         assert target < size
#         assert all(x < size for x in controls)
#         assert target not in controls
#         self.size = size
#         self.controls = controls
#         self.target = target
#
#     def apply_to_row(self, row: row):
#         assert row.size() == self.size
#         assert all(bit in [0, 1] for bit in row.bits)
#         if all(row.bits[i] for i in self.controls):
#             row.bits[self.target] = row.bits[self.target] * -1 + 1
#
#
# class circuit:
#     def __init__(self, gates: list[gate] = []):
#         self.gates = gates
#
#
# g = gate(4, [0, 1], 2)
# r = row([1, 1, 1, 1])
# g.apply_to_row(r)
