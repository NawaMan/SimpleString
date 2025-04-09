# Development Plan

1. Features are to be list in Goals.md
2. Before development, a task file should be create for that future.
3. If it is found that something should be develop before hand,
     a new task should be created for that and developed first.
4. A new feature may BUT should avoid changing existing code -- production or test.
5. The task should be review and revised before development.
6. Only ONE task are to be developed at a time -- it is ok if some sub task cannot or should not be done altogether.
7. A branch should be created for each task
8. Comprehensive set of tests should be written for each task.
9. Commit the task first --- See 12
10. All test must pass
11. The tests should be review and revised.
12. It is ok if we start over  ... 
13. The implemented code should be check against the task spec (in the task file). Mark any that are done.
14. Also mark the complete feature in Goals.md ... if anything left mention that in Goals.md.
15. Create PR and ensure all validation pass.
16. Merge PR


# Release Plan

Release will create binary on all the supported platforms.

After we are happy the set of finished development ...
1. Ensure the version.txt is for a new one (not some that develop before).
2. Run `Release` workflow (on main) to create snapshot ... and ensure that it works.
3. Merge main to release.
4. If things not going as plan, revised it.
5. Add and ensure accuracy of release notes -- CHANGELOG.md.
6. Run `Release` workflow (on release) to actually create the release package.
7. That is it.
