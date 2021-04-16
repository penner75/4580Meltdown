# 4580Meltdown

Created by modifying code from seed security labs meltdown attack lab. The most important file is MeltdownAttack.c.
https://seedsecuritylabs.org/Labs_16.04/System/Meltdown_Attack/


Instructions to use:

Compile CacheTime
- gcc -march=native -o cacheTime CacheTime.c

Keep in mind the threshold speed for something to be accessed from cache.

Next, insert secret data. (can modify size and content of secret, just need to adjust size and change the value)
- make
- sudo insmod MeltdownKernel.ko
- dmesg | grep ’secret data address’

Using the threshold and the secret data address, add these values to the appropriate points in MeltdownAttack.c. Next, compile.
- gcc -march=native -o meltdownAttack MeltdownAttack.c

Now, run it. Can modify code for more or less loops. Tests different (cache threshold) / (number of retries) combinations, providing the average accuracy and speed after 10 attempts.

