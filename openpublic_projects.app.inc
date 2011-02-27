<?php


function openpublic_projects_appstore_app_info() {
  return array(
    'configure form' => 'openpublic_projects_app_configure_form'
  );
}
function openpublic_projects_app_configure_form() {
  $form = array();

  $form['locations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Locations'),
  );
  $form['locations']['location_import'] = array(
    '#markup' => '
    <p>One can tag each project with a City or other location, but the location must first exist in the system.  To add locations one needs to know the longatued and latatude of each location.</p>
    <ul><li>One can inport new locations using a cvs file with three columns (Name, Longaturd, Latatude) on the ' . l(t('Import Location'), 'import/openpublic_projects_locations', array('query' =>drupal_get_destination())). '.</li>
        <li>One can Manage the locations list by hand on the '. l(t('Taxonomy page'), 'admin/structure/taxonomy/location', array('query'=>drupal_get_destination())) . '.</li>
    </ul>'
  );
  return $form;
}
