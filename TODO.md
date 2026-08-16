# Next:
Implement Rendering Interpolation, so the jittering goes away:
    This is caused by the new multithreading, because now updates are not required to happen before rendering.

Also go ahead and move those functions out of the PhysicsManager Header, they do not belong there.
