<?php

// $Id$

/**
 * @file
 * Administrative page callbacks for the filemaker module.
 */

/**
 * Callback for admin settings page (admin/settings/filemaker).
 *
 * @return
 *    Themed HTML table of all the FileMaker connections stored in the Drupal database.
 */
function filemaker_admin_settings_page() {

  $add_connection_link = l('Add a connection', 'admin/settings/filemaker/connection');
  drupal_set_message($add_connection_link);
  
  $head = array('fmcid', 'Hostname', 'Database', 'Username', 'Password', 'Actions');
  $rows = array();

  // Get connections from database.
  $query = db_query("SELECT * FROM {filemaker_connection}");
  $i = 0;

  // Loop through connections and add to each one to the $rows array.
  while ($row = db_fetch_array($query)) {
    $rows[$i] = $row;
    $edit_link = l('Edit', 'admin/settings/filemaker/connection/' . $rows[$i]['fmcid']);
    $delete_link = l('Delete', 'admin/settings/filemaker/connection/' . $rows[$i]['fmcid'] . '/delete');
    $rows[$i]['actions'] = $edit_link . ' | ' . $delete_link;
    $i++;
  }

  return theme_table($head, $rows);
}

/**
 * Page callback for the filemaker connection page page, (admin/settings/filemaker/connection).
 *
 * @return
 *    Form to insert or edit connection and themed HTML table of all the layouts the connection being edited has access to.
 */
function filemaker_admin_settings_connection_page() {

  $fmcid = arg(4);
  $delete = arg(5);

  // Is there a connection to edit or delete?
  if ($fmcid) {

    // If delete, then delete record of connection and recall this page without the the fmcid.
    if ($delete == 'delete') {

      // Delete record.
      $sql = "DELETE FROM {filemaker_connection} WHERE fmcid = %d";
      $result = db_query($sql, $fmcid);
      drupal_set_message(t('FileMaker connection deleted.'));

      // Redirect to a list of all connections.
      $return_path = 'admin/settings/filemaker';
      drupal_goto($return_path);
    }

    // Not deleting a connection.
    else {

      // Get FileMaker object.
      $fm = filemaker_filemaker_object($fmcid);

      // Print out table of layouts.
      if ($fm instanceof FileMaker) {

        $layouts = $fm->listLayouts();

        // Only print out table of connections if we can connect to FileMaker.
        if (!filemaker_is_error($layouts)) {

          drupal_set_message(t('Connected to FileMaker.'));

          $rows = array();
          foreach ($layouts as $layout) {
            $rows[] = array($layout);
          }

          $output .= theme_table(array('Layout'), $rows);
        }
      }
    }
  }
  // Append form to the page.
  $output .= drupal_get_form('filemaker_connection_form');

  return $output;
}
