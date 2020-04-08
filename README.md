What is this project ?
----------------------

This project is **NOT** the official Slicer repository.

It is a fork of Slicer sources hosted at https://github.com/Slicer/Slicer

It is used as staging area to maintain topics specific to [Slicer Custom Application](https://github.com/KitwareMedical/SlicerCustomAppTemplate#readme) that will eventually be contributed back to the official repository.


What is the branch naming convention ?
--------------------------------------

Each branch is named following the pattern `kitware-custom-app-vY.Y.Z-YYYY-MM-DD-SHA{N}`

where:

* `kitware-custom-app` is the name of the custom application. Two cases:
  * can be publicly disclosed: replace with the name of the application (e.g `cell-locator`)
  * can **NOT** be publicly disclosed: use the literal prefix `kitware-custom-app` or `k12345-custom-app` where `12345` is the internal Kitware ID)
* `vX.Y.Z` is the version of Slicer
* `YYYY-MM-DD` is the date of the last official commit associated with the branch.
* `SHA{N}` are the first N characters of the last official commit associated with the branch.

For more details, see https://www.slicer.org/wiki/Documentation/Nightly/Developers/ProjectForks

What is the relationship between Slicer/Slicer and Slicer/SlicerGitSVNArchive ?
-------------------------------------------------------------------------------

Following the [transition](https://discourse.slicer.org/t/transition-to-github/10358) to using GitHub as the only repository for organizing Slicer sources, former Slicer GitHub repository has been renamed to [Slicer/SlicerGitSVNArchive](https://github.com/Slicer/SlicerGitSVNArchive).

To ensure existing Kitware projects can still build, this repository was **NOT** renamed. That said, new branches related to the new [Slicer/Slicer](https://github.com/Slicer/Slicer) repository should still be published in this fork.

