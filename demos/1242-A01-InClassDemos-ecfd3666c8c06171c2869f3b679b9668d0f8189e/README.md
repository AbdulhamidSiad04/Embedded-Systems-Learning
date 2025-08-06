# Section A01 - Winter 2025 Demos

This repository will be populated with demo projects by your instructor throughout the term. It has been added to your home repository as a submodule. Because of this, it will not update when you run a typical ```git pull``` command. You should update it from your home directory with a ```git submodule update --init --remote``` or ```git submodule update --remote``` command. You will only need the ```--init``` option the first time you update this submodule. After updating the submodule, you will need to commit and push the change in your home repository. 

In general, run the following commands from your home directory to update the demo code:
```
git submodule update --init --remote
git add .
git commit -m "submodule update"
git push
```

More information on submodules can be found in the [git-scm.com docs](https://git-scm.com/docs/git-submodule) and the [git-submodule man pages](https://www.kernel.org/pub/software/scm/git/docs/git-submodule.html)

If you are having issues updating this repository or would like a specific demo, please speak with your instructor. 
