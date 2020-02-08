![C/C++ CI](https://github.com/patrickgue/timekeeper/workflows/C/C++%20CI/badge.svg)

Timekeeper
----------

This application is used to track time on the things you're working on. This program has a simple command line interface. Here you can see a few examples:


**Start:**

To open a new entry you can type

`$ tk start "Implement feature XYZ of project ABC"`

**Stop:**

`$ tk stop`

You can change a comment when stopping an entry. 

`$ tk stop "Implement feature XYZ of project ABC"`


**New:**

If you want to transition seamlessly to a new entry you can use:

`tk new "Implement another feature of project ABC"`

You have to have an open entry for this to work.

**List:**

Display all entries:

```
$ tk list 
(000) S: 02-08 10:29:02   E: 02-08 12:09:36   D: 01:40:34 (Implement feature XYZ of project ABC)
(001) S: 02-08 12:22:56   E: 02-08 13:17:47   D: 00:54:51 (Implement another feature of project ABC)
(002) S: 02-08 13:17:47   Current             D: 00:01:17 (Implement different feature of project ABC)
```


**Status:**

```
tk status
(002) S: 02-08 13:17:47   D: (00:02:39)   (Implement different feature of project ABC)
```

