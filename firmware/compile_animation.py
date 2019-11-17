# Compiling animation into bit encodin representation.
# Each Frame contais 20 states for each LED.
# State could be 1 (LED shine) or 0 (LED extinguished).
# To store a full Frame we just need 20 bits (2.5 bytes).
# Animation file could contain multiple animation divided by a line.
# After each preprogramed animation will be displayed "stars".

import re
import itertools

RE_PATTERN = r'\{' + ','.join(["([01])"] * 20) + r'\}'
ANIMATION_SPLITTER = '==='

def chunkit(array, chunk_size):
    for i in range(0, len(array), chunk_size):
        yield array[i : i + chunk_size]

with open('src/animation.txt', 'r') as animation_file:
  animation_sources = animation_file.read().split(ANIMATION_SPLITTER)

animation_units = [re.findall(RE_PATTERN, pattern_string) for pattern_string in animation_sources]
animation_units = [i for i in animation_units if i]
animation_unit_length = [len(i) for i in animation_units]
animation_unit_end_frame = [sum(animation_unit_length[:i + 1]) for i, length in enumerate(animation_unit_length)]
animation_led_states = itertools.chain(*itertools.chain(*animation_units))
full_animation = [int(i) for i in animation_led_states]

compiled_animation_bytes = []
for data_chunk in chunkit(full_animation, 8):
    value = sum( (1 << i) * value for i, value in enumerate(data_chunk))
    compiled_animation_bytes.append("0x%0.2X" % value)

print('-DCOMPILED_ANIMATION="{%s}"' % ','.join(compiled_animation_bytes))
print('-DANIMATION_END_FRAMES="{%s}"' % ','.join(str(i) for i in animation_unit_end_frame))