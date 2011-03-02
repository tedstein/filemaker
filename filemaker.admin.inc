<?php




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





/**
 * Form to add or edit a single FileMaker connection.
 */
function filemaker_connection_form() {

  $fmcid = arg(4);

  // Record being inserted or updated?
  if ($fmcid) {
    $fmc_sql = "SELECT * FROM {filemaker_connection} WHERE fmcid = %d";
    $fmc = db_fetch_object(db_query($fmc_sql, $fmcid));
  }

  $form = array();

  // Insert validation, submit function names, and redirect location.
  $form['#submit'][] = 'filemaker_connection_submit';
  $form['#redirect'] = 'admin/settings/filemaker';

  // Create connection fieldset.
  $form['connection'] = array(
    '#title' => t('FileMaker Connection'),
    '#type' => 'fieldset',
  );

  // Hostname field.
  $form['connection']['fmhostname'] = array(
    '#title' => t('Host Name'),
    '#type' => 'textfield',
    '#default_value' => $fmc->fmhostname,
  );

  // Database field.
  $form['connection']['fmdatabase'] = array(
    '#title' => t('Database Name'),
    '#type' => 'textfield',
    '#default_value' => $fmc->fmdatabase,
  );

  // Username field.
  $form['connection']['fmusername'] = array(
    '#title' => t('Username'),
    '#type' => 'textfield',
    '#default_value' => $fmc->fmusername,
  );

  // Password field.
  $form['connection']['fmpassword'] = array(
    '#title' => t('Password'),
    '#type' => 'textfield',
    '#default_value' => $fmc->fmpassword,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Connection'),
  );

  return $form;
}





/**
 * Submit handler for filemaker_connection_form.
 */
function filemaker_connection_submit(&$form, &$form_state) {

  $fmcid = arg(4);

  $fmhostname = $form_state['values']['fmhostname'];
  $fmdatabase = $form_state['values']['fmdatabase'];
  $fmusername = $form_state['values']['fmusername'];
  $fmpassword = $form_state['values']['fmpassword'];

  // Update connection?
  if ($fmcid) {
    $sql = "UPDATE {filemaker_connection} SET fmhostname = '%s', fmdatabase = '%s', fmusername = '%s', fmpassword = '%s' WHERE fmcid = '%d'";
    db_query($sql, $fmhostname, $fmdatabase, $fmusername, $fmpassword, $fmcid);
    drupal_set_message(t('Connection updated.'));
  }

  // Insert connection.
  else {
    $sql = "INSERT INTO {filemaker_connection} (fmcid, fmhostname, fmdatabase, fmusername, fmpassword) VALUES (NULL, '%s', '%s', '%s', '%s')";
    db_query($sql, $fmhostname, $fmdatabase, $fmusername, $fmpassword);
    drupal_set_message(t('Connection Created.'));
  }
}
