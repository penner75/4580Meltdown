#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#define LENGTH 8

//lengths 8, 101, 1025, 2049

//static char secret[LENGTH] = {'S','E','E','D','L','a','b','s',};
//static char secret[LENGTH] = "2!awYwEIP*a233PppjzO4QfvLMGHh^^KHhUVsX-Sj+!KjZ1YZ@Yn2Kp-9%D1I**Tw9VJwUBD309hr^?3470$-SXu0M%#BAo_#|wG";
//static char secret[LENGTH] = "WZBYMdrLYhPq2scQRR6D8rJTxy56G2HPuXCEAfSSf4LJ9WT3xAWtBAKbjXwXzR9JyJPLQH72AJ59Fedw4xSS3SANTFcHaST2DLAn5Ga7vdpSUSGRWgK2gHpA9nFAVatUeVvWvTZFwJ3BcYa42a5ZbnqN3TtErPMScaKHE9xDTEmF7PnzpkVtMsyx6T4yFTZhep9E2AkKu8aFaTBRZVre2XpdQBuGWPF5VjzCa9pgP6atwB6dDBpSkvaxK63T7R23GUtyBAaUbhEffJhaSYFrWBbfSXapKnEVgjeBaEGTyceDYKjzxszuDVNJcv7E3WnDBPYNquuETQ6jnSfwFvEHkQUkYKTXWWbRAgNh5KyWwmGyJvJBStRjbrkXqDKC3PH5Z4tABJKmg6pcWpFKpdaEaC4K3D32DJJU6sL5dE7wZHdGGhSs3tnhLEWAh4a29NDPNYcgGBRdfXGuGjLLWFAKnYPNsgdEybTNzvwQC6F8tdKfQvrch3xQ7zdY6u4mprYY3fhZFeHKNmuSQPzejg4TLTLV3Rgf4PVexCNjWkKwFRykyC65Ww8sNQnpGUJsJRjFH5MPxcreXLm3KNvC4xRwVq9Nc7ECdzQqTdmGW92Tm52AXS8ezyrzqdeeJuSZE8ZTR8wxg2WkLcJxEhD92GxtyTDUXgYRabyuQVwrvFvwHAsjAPub7dM84ckkQ9L26eMEVyAvZhtH76VfQ3pSqKLbLMNLKjjgshMxJ2Wn5NvcPxkYfwKDYFRG3nMVB5UrrvbvzmVqkhs5zS3n9AECgcB4Gc7XzJsXy4GTbmLUTDYkqCwfaRe2NKgunpL6SHNPNR3tRCVpJj7FqdDr8rxRFJnb5PuuH6h4rXTtGNHAachQQhubX8t8RhaXZeLrhckhg6qTvdSqCczXLzQCkjXNtVAb4HHJr5KH5pHfDnzLcVdZYDzeNK5UvZ7DzJ9ax3mzJVxqX9t7C5egGqeJtfBYwSVZjJSaqnf9D3L6YkerhewjZFPgY6R6RHDqD6z8j7GGgED3";
static char secret[LENGTH] = "D2V8saQfN8GGLYQvsNxj2mq9FyYKmBYttjBW2qhsKc4Lges8ACWQbmw7jKzZmL2AuFMfbgfQYspPY29jdkczktbuqPFEV6wv3GfKSMB5746KhwrtH79dtrxT9XJXXyzPyrgwWTqmC9aUSErW48kFEexthj9X36bCe6vRk2KTurhuKnmfYkaKxzMaCrACenaM3B6AaGheUXbRajdBbNGqtvXQ3tsSCEBz5KABRtCPENGa5KeKdBUqkJCWLfbPs67M7NnnRUpM9MKCwgdXfavQGDueB3P8GW9aZDUL5x4M6qhhnLAeKzjMsVWKGAzmG2BufGAJnjMfHDEFcyAJfF2khZP2qhUgdagfxpsVkAkMacP9CTcECMVckLWB9tUz4SLsg2uabXXjAY9FsDPvpVU3KDjb7Qna46fXC9aezHCGXEereYD8wkfRJcvzWMPrZtMw5bzT3h4bre7Z39f7Vjb2GFNYqRhM6hrYD5kw6jSgJGVDxke2ZGrLsgjJbUFPfdNWUSPgzfx4ba7uvUgBC3yUKBkB4kgYV3QMtBcqVbsNuapxGLQkMbEVbfLPTAVPaJTTLxdtKpz6M2W5QNvqpJrgpCARsvzKfagzUhZFT96yrX34bNEyV2tV4qFqzeZjhWaxxChNHm8zTsbbQJJMW85NjYYYqhj9wYFTvdsYJ377PbcYt3SfCPaSHaCCrPrZKfwcsZPuYGMHH7fhgk5hUuYckRjN6thBEdMT9UynJXxaN2fncnDhSEuCrcmWFct9SKT7KNpbP7ZYpP8xVYXaUNEFXJbrC6h6u8g5p95cfVtaVatvFugSQLJnRzZ28Kv6wSKUTL2ES9k94khDbYLkyF9kQJMbVCyeVeduKh6UT8gB7skMjvzVTCby52DunVyveCJTpezC2cM9uCvxJbQst3WWRxRq7EYP8KHnNswMxZNWPZ4mbkbRb3skQQHUxta9B2SuS6PWfHPzYkvdGZBdXms4xFx6VerAGQun2wxJJahMcLKun89R4mdJpVPjTqYENv3huSVeKdv4fT9ZCUDPj5R25AMJBpFeEw4rcrEspmeLWp2UrwtvASRtyTBGzUZkK5GusCj8exMbzGKpg7fX2XExkgSRvMpepguhYrUC5wFNaumjmNzaVgfnSTkQ6Mx4CkbsR7MgQKmwMRUNVAuSK5L44qSK92KhaGY2E3JfzsmkGQBwxr9vFSMqTDvpwAf2SrJTtL82XLLe9GwXT5G2pYyq3uzzbPxe8aWESJ2KLm6jgUkvw2VEYxCNALkQbVcNfu2SPbVqCtHMRhq45zDndqEtmtkuQZs6aLJBW5y38vuTXdDbeQXEBcyMtpScRbtXCWzw3CUHURhDHrUctd43UXTyXrwDEcyNUg3sjU4TjDhw39eLeVLhm8mg4tXPQ6yWP7DfcKaBhUjRxmU8HUWrH4SrXVpNee7nG4kZLgVYgYWaYRQJTMhPvVcQsaGhSHcsjS9zjuEUnNhnN5QgvDmKFUFWemfDb86zaHCazpRYX7n5SxQ869a34ncVCNSdwhT7CWVbjJNCUekxw6Q38fvAuDzwqXJTpjPAGw8WSCCwqahmwJPKSsKLyNhqqN6ZNGzAmnnb68ppH2cYE4yDNHkY2t8jP28L4ZGPbQrnvykZdwPQZ83pzmW3cBLmffUkCKQnXupZqKaX4EZcRAnbNsa92PKmE3a3yBGcxz9xkKBedSEJmWDKeUhvAjm53MtUPqrCq5qEYYGM6WzyWxDhcHBvLTF9wrJLpNpDfRGTyc2g93XT5KEKnWd5Pttpm9uG8Pyjz2g7fsFNtRBYKMmGJ9vhYykjNZfpy3K2tsFTbzs4tUf2TdxQfF7eYWgQNEtYW9LTu6ppsrjHcHnX98kFbcYs8NTYxYcB4UtfN9UDYRyWEp6EHLZVSye83QbeGvsmzCDAtvN4D3b48tE5YBFeeJkFVmtqhyZecR2McWFqPRc446r5H8A7HabLghPxcDjTmDuzsrnyCetpGkGJKSRhFfWuDfnS8epQH2J894rA2pRNHAbjtftdNmuKnsMWbyZKh8mckvZW";
static struct proc_dir_entry *secret_entry;
static char* secret_buffer;

static int test_proc_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4,0,0)
   return single_open(file, NULL, PDE(inode)->data);
#else
   return single_open(file, NULL, PDE_DATA(inode));
#endif
}

static ssize_t read_proc(struct file *filp, char *buffer, 
                         size_t length, loff_t *offset)
{
   memcpy(secret_buffer, &secret, LENGTH);              
   return LENGTH;
}

static const struct file_operations test_proc_fops =
{
   .owner = THIS_MODULE,
   .open = test_proc_open,
   .read = read_proc,
   .llseek = seq_lseek,
   .release = single_release,
};

static __init int test_proc_init(void)
{
   // write message in kernel message buffer
   printk("secret data address:%p\n", &secret);      

   secret_buffer = (char*)vmalloc(LENGTH);

   // create data entry in /proc
   secret_entry = proc_create_data("secret_data", 
                  0444, NULL, &test_proc_fops, NULL);
   if (secret_entry) return 0;

   return -ENOMEM;
}

static __exit void test_proc_cleanup(void)
{
   remove_proc_entry("secret_data", NULL);
}

module_init(test_proc_init);
module_exit(test_proc_cleanup);
