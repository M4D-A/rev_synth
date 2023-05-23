from itertools import product


class row:
    def __init__(self, bits: list[int]):
        assert all(bit in [0, 1] for bit in bits)
        self.bits = bits

    def size(self):
        return len(self.bits)

    def __str__(self):
        string = ""
        for b in self.bits:
            string += str(b) + ","
        return string


class truth_table:
    def __init__(self, size):
        base_bits = [0, 1]
        self.table = \
            [row(list(bits)) for bits in product(base_bits, repeat=size)]


class gate:
    def __init__(self, size, controls: list[int], target: int):
        assert target < size
        assert all(x < size for x in controls)
        assert target not in controls
        self.size = size
        self.controls = controls
        self.target = target

    def apply_to_row(self, row: row):
        assert row.size() == self.size
        assert all(bit in [0, 1] for bit in row.bits)
        if all(row.bits[i] for i in self.controls):
            row.bits[self.target] = row.bits[self.target] * -1 + 1


class circuit:
    def __init__(self, gates: list[gate] = []):
        self.gates = gates


g = gate(4, [0, 1], 2)
r = row([1, 1, 1, 1])
g.apply_to_row(r)
print(r)
