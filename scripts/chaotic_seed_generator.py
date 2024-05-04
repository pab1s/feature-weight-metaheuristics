import numpy as np
from hashlib import sha1

lavaImage = './data/perro.jpg'

def generate_number(low, high, size, image_path):
    # Hash the image produced by physically random image
    hashed_image = sha1(open(image_path, 'rb').read()).hexdigest()
    seed_val = int(hashed_image, 16)
    
    # Use the hashed image and seed value to create a random number generator
    rng = np.random.default_rng(seed_val)
    random_integers = rng.integers(low=low, high=high, size=size)
    
    return random_integers

# Generate a single random integer in the range of 0 to 2^31 - 1
random_integer = generate_number(0, 2**31 - 1, 1, lavaImage)
print(random_integer[0])
