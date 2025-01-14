import pandas as pd
from build.MyLib import MyClass, adder, free_function, module_a, zeroes
import build.MyLib as m
import matplotlib.pyplot as plt

Simulation = MyClass(-4, 4, 1000)
Simulation.run()

plt.plot(Simulation.v_data, Simulation.v_gamma, "--", linewidth=3, color=(1, 0, 0, 0.6), label="Function Value")

a = pd.DataFrame(Simulation.v_data)

plt.ylim(-20, 10)
plt.xlim(-20, 10)
plt.xlabel("x")
plt.ylabel(r"($f(x) = \gamma(x)$)")
plt.title(r"(Gamma Function: $\gamma(z) = \int_0^\infty x^{z-1} e^{-x} dx$)", fontsize=18)

plt.savefig("test.png")

# Use the add function from the adder submodule
sum_result = adder.add(1, 2)
print(f"The result of adding 1 and 2 is {sum_result}")

print(f"{free_function()}")
print(f"{module_a.func()}")
print(adder.__doc__)  # Output: This is the submodule docstring

zeroes.show_zeroes(5, 5)

v = Simulation.vector_getter()
print(type(v), v)

mat = m.eigen_sample()
print(mat)
print(type(mat))

vec = m.use_class_get_vector()
print(vec)
print(type(vec))

print("")
print("Boost sample")
m.boost_tokenizer()

print("")
print("Return tensor")
tensor = return_tensor()
print(return_tensor.__doc__)  # Output: This is the submodule docstring
print(type(tensor))
print(tensor)