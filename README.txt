// $Id$

***********************************************************
Features
***********************************************************

- Unlimited number of reusable connections to FileMaker databases
- Create, find, browse, and edit modes
- Rich set of permissions
- Choice of interface for anonymous users
- Support for Drupal actions
- Support for FileMaker portals
- Support for all types of FileMaker value lists

***********************************************************
Requirements (FileMaker)
***********************************************************

- FileMaker server
- FileMaker user account with with PHP extended privileges
- FileMaker API for PHP (which came with your copy of FileMaker server)

***********************************************************
Dependencies (Drupal)
***********************************************************

- Libraries module

***********************************************************
Installation instructions
***********************************************************

- Create a directory in sites/all/libraries named filemakerapi.

- Place the FileMaker API in the filemakerapi folder.

  sites/all/libraries/filemakerapi/FileMaker/
  sites/all/libraries/filemakerapi/FileMaker.php
  
- Install the FileMaker and Libraries module.

  sites/all/modules/libraries
  sites/all/modules/filemaker

- Enable the modules at admin/build/modules.

***********************************************************
Permissions
***********************************************************

Unless you are user #1, you will not be able to access the FileMaker tabs.

Permissions:

- FileMaker nodes -- Create, edit, and delete FileMaker nodes (similar to FileMaker layouts).
  This permission grants a user the right to create a FileMaker node based on any of your FileMaker 
  connections, but the user can not create, edit, or delete any connections.

- administer filemaker -- Create, edit, and delete FileMaker connections. 
  This permission grants a user the right to see FileMaker passwords.

- browse and find mode -- Performs finds on, edit, and delete records inside of a FileMaker database.

- create mode	-- Create new records inside of a FileMaker database.

- layout mode -- Modify FileMaker nodes by adding or modifying fields and portals.

***********************************************************
Create connection(s) to FileMaker database(s)
***********************************************************

To interact with FileMaker you must first add a FileMaker connection at admin/settings/filemaker, accessible via the Drupal menu at:

Administer > Site Configuration > FileMaker Settings.

The username and password are the FileMaker username and password, and you will only have access to the FileMaker elements that the FileMaker account has.

The user account you use must have fmphp Extended Privileges and the file must be hosted on a FileMaker server.

Do not use the .fp7 suffix for your database name. my_database.fp7 should be entered as my_database.

You may add as many connections, to as many databases, as you want. Each node will be based on exactly one connection.

***********************************************************
Prepare FileMaker for the web
***********************************************************

Create layouts in FileMaker which include the fields/portals you want to appear on the web.

Every filemaker node is tied to a single layout and will only be able to interact with elements on that layout.

Be thoughtful when adding elements to the layout in FileMaker, as performance will decrease dramatically if you add too many.

It will almost certainly be better to have separate layouts for each node than to try to reuse a layout by adding more elements to it.

***********************************************************
Create filemaker nodes and interact with FileMaker:
***********************************************************

Create a filemaker node, like any other piece of content (node/add).

The only additional field on the node creation screen is a select field so you choose a connection (see above) and save the node. 

Once the node is saved, visit the 'Layout Mode' tab on the node, choose which FileMaker layout the node will be based on, and add the fields and portals you want. 

That's it. Now you can enter 'Find Mode' and search for records, 'Create Mode' to add records, or 'Browse Mode' to view and edit records.
