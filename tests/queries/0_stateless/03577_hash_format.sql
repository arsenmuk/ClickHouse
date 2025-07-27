SELECT number FROM system.numbers LIMIT 1 FORMAT Hash;
SELECT number FROM system.numbers LIMIT 20 FORMAT Hash;
SELECT number AS hello, toString(number) AS world, (hello, world) AS tuple, nullIf(hello % 3, 0) AS sometimes_nulls FROM system.numbers LIMIT 20 FORMAT Hash;

SET output_format_hash_function = 'sipHash128';
SELECT number AS hello, toString(number) AS world, (hello, world) AS tuple, nullIf(hello % 3, 0) AS sometimes_nulls FROM system.numbers LIMIT 20 FORMAT Hash;

SET output_format_hash_function = 'sipHash64';
SELECT number AS hello, toString(number) AS world, (hello, world) AS tuple, nullIf(hello % 3, 0) AS sometimes_nulls FROM system.numbers LIMIT 20 FORMAT Hash;

SET output_format_hash_function = 'cityHash128';
SELECT number AS hello, toString(number) AS world, (hello, world) AS tuple, nullIf(hello % 3, 0) AS sometimes_nulls FROM system.numbers LIMIT 20 FORMAT Hash;

SET output_format_hash_function = 'cityHash64';
SELECT number AS hello, toString(number) AS world, (hello, world) AS tuple, nullIf(hello % 3, 0) AS sometimes_nulls FROM system.numbers LIMIT 20 FORMAT Hash;

SET output_format_hash_function = 'murmurHash3_128';
SELECT number AS hello, toString(number) AS world, (hello, world) AS tuple, nullIf(hello % 3, 0) AS sometimes_nulls FROM system.numbers LIMIT 20 FORMAT Hash;

SET output_format_hash_function = 'murmurHash3_64';
SELECT number AS hello, toString(number) AS world, (hello, world) AS tuple, nullIf(hello % 3, 0) AS sometimes_nulls FROM system.numbers LIMIT 20 FORMAT Hash;

SET output_format_hash_function = 'xxHash64';
SELECT number AS hello, toString(number) AS world, (hello, world) AS tuple, nullIf(hello % 3, 0) AS sometimes_nulls FROM system.numbers LIMIT 20 FORMAT Hash;
