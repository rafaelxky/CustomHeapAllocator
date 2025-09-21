What it can do now:
Currently you can allocate and dealocate memory
It reuses heap memory if not in use as well as sections
Does not reorder memory yet

Todo:
Limit searches so that it only looks within the offset and not the entire registry
Reorder memory logic
Remove any mmalloc calls
Make it safer to use and avoid memory issues
Write README.md

psudocode for memory ordering
get a section
check if memory before is in use
continue untill is not
move whole memory back
update starrt value