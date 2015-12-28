# avr_misc_sketches
All misc AVR app code, that is worth commit but not separate repo.

## Repository organisation.

**master** branch is not aim to host a collection of project.

Instead it should be kept clean (maybe I'll find it a smart use in the future :) ).


### Orphan branch for each sketche.

When creating a new project please create its orphan branch associated, and empty repo content : 

    git checkout --orphan new_project_branch_name 
    git rm *
    git rm -f src/*
    # ... possiblity add some files for an initial commit ...
    git commit -a -m "Initiatied new project : short project description"


**Note:** it's no problem to have sub-branches for each sketches/project.
The aim of orphan branch is to clearly 

### List of hosted sketches 

 * scheduler : some tries about writing "context switching" for AVR... Toward thread scheduling on Arduino ? ^^
 * 


