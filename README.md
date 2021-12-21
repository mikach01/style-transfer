This is a small project done for our research in computer science class with Nicolas Bonneel.

I use some notions of optimal transport like the sliced wasserstein distance in order to do a style transfer between two images.

This project uses the stb_image library.

# Compilation

Create a build directory for cmake:

```
mkdir build
cd build
```

run cmake and make:

```
cmake ..
make
```

# Launch

run the executable: `./wasserstein`

You can specify some parameters like the two images used for style transfer and the number of iterations.

ex: `./wasserstein ../data/imgA.jpg ../data/imgB.jpg 20`

You can find the result under the name of `output.png`.
