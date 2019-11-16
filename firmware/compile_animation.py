import re
import itertools

def chunkit(array, chunk_size):
    for i in range(0, len(array), chunk_size):
        yield array[i : i + chunk_size]

with open('src/animation.txt', 'r') as animation_file:
  pattern_string = animation_file.read()

re_pattern = r'\{' + ','.join(["([01])"] * 20) + r'\}'
animation_units = re.findall(re_pattern, pattern_string)
full_animation = [int(i) for i in itertools.chain(*animation_units)]

compiled_animation_bytes = []
for data_chunk in chunkit(full_animation, 8):
    value = sum( (1 << i) * value for i, value in enumerate(data_chunk))
    compiled_animation_bytes.append("0x%0.2X" % value)

print('-DCOMPILED_ANIMATION="{%s}"' % ','.join(compiled_animation_bytes))