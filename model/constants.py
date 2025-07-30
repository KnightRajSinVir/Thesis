bls12_381 = 0x1A0111EA397FE69A4B1BA7B6434BACD764774B84F38512BF6730D2A0F6B0F6241EABFFFEB153FFFFB9FEFFFFFFFFAAAB  
bls12_381_modulo_inverse = 0x627A0CEB06106FEAAFC9468B316FEE268CF5819ECCA0E8EB2DB4C16EF2EF0C8E30B48286ADB92D9D113E889F3FFFCFFFCFFFD  
R = 1 << (13 * 31)
p = bls12_381
from math import ceil
print(
    (
        p + (((2*p - 1)**2) >> (13 *31)) + 2
    ) // p
)
print(
    (
        (
            ( 
                (4*p - (((2*p - 1) ** 2) >> (13 * 31)) - 3) * (p - 1) 

            ) >>(13*31) 
        )
        + p
    ) // p
)

print( (2 * p - 1) ** 2)
print( ((2 * p - 1) ** 2) >> (13 * 31))
result = ((((2 * p - 1) ** 2) >> (13 * 31)) // p)
print(int(result))  # explicitly integer division

print( ((((2 * p - 1) ** 2) >> (13 * 31) ) / p))