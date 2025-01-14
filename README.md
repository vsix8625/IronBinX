### IronBinX project

## About This Project

This custom build tool is a work in progress. 
I’ve had a blast building this tool and diving into the world of build systems!
While my tool may not yet reach the levels of established solutions like CMake or Make,
it serves as a great learning platform and offers unique utilities tailored to my projects.

I started coding in 2023, and while I’m excited to share my efforts, I am still learning and may encounter issues in the code. 
Feedback, suggestions, and contributions are welcome as I continue to improve my skills!

## Features 
- Alias: ibx
- Options: 
          --init: Initializes ibx in current dir.  
   -i     --install   
   -b     --build: Build an executable based on the config/build.ibx  
   -r     --run: Run the executable created by ibx.
   -bo    --build-objects: Build objects files from all .c files  
   -lo    --link-objects: Link all objects files in build/objects dir.  
   -bolo  --build-objects-link-objects: Build objects files and link them.  
   -cl    --clean: Send all files inside bin dir to .ironbinx/trash dir.  
   -et    --empty-trash: Permanently removes contents inside .ironbinx/trash dir.  
   -st    --show-trash: Show the contents inside .ironbinx/trash dir.  
   -sb    --show-bin: Show the contents of bin dir.  
   -sbf   --show-build-file: Show the contents of config/build.ibx.  

   You can run --install ibx in an existing C project.
          This command will create necessary files and directories for ibx to run.  
          A build config file will be created in  config/build.ibx  with key: value pairs.  
          You can modify values the build.ibx file that will be used with --build and other cmds.  
          "compiler": "gcc"; key, values must be enclosed in double quotes (")  
              and semicolon(;) is required at the end of each pair.  
          Some commands can be used in a single call each will wait the last to end.  
          In a newly --make <ProjectName> you can cd <ProjectName> then run `ibx --init --build --run`.    
          This will initialize IronbinX then build the project and run it.
          Another example is `ibx --clean --build`.
              This cmd will remove bin dir and build a new executable.
          Bin dirs removed with clean will be moved inside .ironbinx/trash with a timestamp of deletion.


## Usage:
  - To build a new project: `ibx --make <ProjectName>` or `ibx -m <ProjectName>`.  
  - Then `cd <ProjectName>`.  
  - Finally `ibx --init`.  





