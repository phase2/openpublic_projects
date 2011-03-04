<?php


function openpublic_projects_apps_app_info() {
  return array(
    'configure form' => 'openpublic_projects_app_configure_form',
    'post install callback' => 'openpublic_projects_post_install'
  );
}

/*
 * Configure form for the app page
 */
function openpublic_projects_app_configure_form() {
  $form = array();

  $form['locations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Locations'),
  );
  $form['locations']['location_import'] = array(
    '#markup' => '
   <p>One can tag each project with a City or other location, but the location must first exist in the system.  To add locations one needs to know the longitude and latitude of each location.</p>
   <ul><li>One can import new locations using a cvs file with three columns (header row is "name", "lat", "lon") on the ' . l(t('Import Location Page'), 'import/openpublic_projects_locations', array('query' =>drupal_get_destination())). '.</li>
      <li>One can Manage the locations list by hand on the '. l(t('Taxonomy page'), 'admin/structure/taxonomy/location', array('query'=>drupal_get_destination())) . '.</li>
   </ul>'
  );

  $files = scandir(drupal_get_path('module', 'openpublic_projects') . "/locations/");
  array_shift($files);
  array_shift($files);

  $options = array_combine($files, array_map(create_function('$a' , 'return substr($a, 0,2) . " (".count(file(drupal_get_path("module", "openpublic_projects") . "/locations/$a")).")";'), $files));
  $form['locations']['location_file'] = array(
    '#title' => "Import Cities From:",
    '#type' => "select",
    '#options' => $options,
    '#description' => 'Import all of the cities in one state into locations',
  );
  $form['locations']['location_file_import'] = array(
    '#value' => "Import",
    '#type' => "button",
    '#submit' => array('openpublic_projects_app_configure_form_submit_import_location'),
    '#executes_submit_callback' => TRUE,
  );
  $form['projects'] = array(
    '#type' => 'fieldset',
    '#title' => t('Projects'),
  );
  $form['projects']['projects_import'] = array(
    '#markup' => 'One can create new projectis on the ' . l(t('Create Project Page'), 'node/add/project', array('query' =>drupal_get_destination())) . ".",
  );
    
  return $form;
}
function openpublic_projects_app_configure_form_submit_import_location($form, &$form_state) {
  openpublic_projects_import($form_state['values']['location_file']);
}

/**
 * Imports a CSV file.
 */
function openpublic_projects_import($file) {
  $importer = 'openpublic_projects_locations'; 
  $filepath = "public://openpublic_projects/";
  file_prepare_directory($filepath, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);
  $filepath .= $file;

  // Copy locations csv to file directory, avoid using shell_exec() for
  // compatibility reasons. For security reasons, feeds only consumes files
  // from the public directory.
  if (!file_exists($filepath)) {
    $src = file_get_contents(drupal_get_path('module', 'openpublic_projects') . "/locations/{$file}");
    if($src) {
      file_put_contents($filepath, $src);
    }
    else {
      drupal_set_message(t('Error importing content. No such file ' . $file), 'error');
      return;
    }
  }
  try {
    // Ensure importer exists.
    feeds_importer($importer)->existing();
    $source = feeds_source($importer);
    $source->addConfig(
      array(
        'FeedsFileFetcher' => array(
          'source' => $filepath
        ),
      )
    );
    while (FEEDS_BATCH_COMPLETE != $source->import());
  }
  catch (Exception $e) {
    drupal_set_message(t('Error importing demo content. ' . $e->getMessage()), 'error');
  }
}
function openpublic_projects_post_install() {
  openpublic_projects_import('US.csv');
}
