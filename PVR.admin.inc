<?php

/**
 * Callback hook_menu
 *
 * @param array $form
 * @param array $form_state
 */
function PVR_configure_form ($form, &$form_state) {
	$form['login'] = array(
		'#type' => 'textfield',
		'#title' => t('Login'),
		'#default_value' => variable_get('PVR_login', ''),
		'#required' => TRUE,
	);
	$form['password'] = array(
		'#type' => 'password',
		'#title' => t('Password'),
		// '#default_value' => variable_get('PVR_password', ''),
		'#attributes' => array('value' => variable_get('PVR_password', '')),
		'#required' => TRUE,
	);

	$form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

	return $form;
}

/**
 * Implement hook_validate
 * Validate the PVR_configure form
 * 
 * @param array $form
 * @param array $form_state
 */
function PVR_configure_form_validate($form, &$form_state) {
	if (!PVR_check_login_password($form_state['values']['login'], $form_state['values']['password'])){
		form_set_error('password',t('The login or password is not correct'));
	}
}

/**
 * Implement hook_submit
 * Submit the PVR_configure form
 * 
 * @param array $form
 * @param array $form_state
 */
function PVR_configure_form_submit($form, &$form_state) {
	variable_set('PVR_login', $form_state['values']['login']);
	variable_set('PVR_password', $form_state['values']['password']);

	drupal_set_message(t('Your configuration has been saved.'));
}

/**
 * Callback hook_menu
 *
 * @param array $form
 * @param array $form_state
 */
function PVR_project_list_form ($form, &$form_state) {
	$projects = PVR_get_projects_for_user();
	$form['projects']['#weight'] = 10;
	$form['projects']['#tree'] = TRUE;

	foreach ($projects as $key => $project) {
		$form['projects'][$key] = array(
			'id' => array(
		    	'#markup' => $project['project_id'],
		    ),

			'title' => array(
		    	'#markup' => $project['project_name'],
		    	// '#markup' => 'ㄹ오ㅓㄴㄹㅇㅗㄹㅎ',
		    ),

		    'operations' => array(
		      	'#markup' => l(t('Edit'), 'admin/config/PVR/project/'.$project['project_id'].'/edit') . ' - ' . l(t('Delete'), 'admin/config/PVR/project/'.$project['project_id'].'/delete')
		    ),
		);	
	}

	$form['link'] = array(
		'#markup' => '<ul class="action-links"><li><a href="#">'.t('Add a new project').'</a></li></ul>',
		'#weight' => 0
	);

	$form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

	return $form;
}

/**
 * Implement theme_THEME for PVR_project_list_form
 * 
 * @param array $variables
 */
function theme_PVR_project_list_form($variables) {	
	$form = $variables['form'];
  	$rows = array();
  	foreach (element_children($form['projects']) as $index) {
	    $rows[] = array(
			'data' => array(
				drupal_render($form['projects'][$index]['id']),
				drupal_render($form['projects'][$index]['title']),
				drupal_render($form['projects'][$index]['operations']),
			),
			// 'class' => array('draggable'),
	    );
  	}

  	$header = array(t('ID'), t('Title'), t('Operations'));
  	$table_id = 'pages-table';
  	$output = theme('table', array(
    	'header' => $header,
    	'rows' => $rows,
    	'attributes' => array('id' => $table_id),
  	));

  	$output .= drupal_render_children($form);
  	return $output;
}

/**
 * Callback hook_menu
 *
 * @param array $form
 * @param array $form_state
 * @param string $id
 */
function PVR_project_edit_project_form ($form, &$form_state, $id) {
	$iframe = PVR_get_iframe_project_edit($id);
	
	$form['iframe'] = array(
		'#markup' => $iframe,
	);
	

	$form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

	return $form;
}

/**
 * Implement hook_validate
 * Validate the PVR_project_edit_project_form form
 * 
 * @param array $form
 * @param array $form_state
 */
function PVR_project_edit_project_form_validate($form, &$form_state) {}

/**
 * Implement hook_submit
 * Submit the PVR_project_edit_project_form form
 * 
 * @param array $form
 * @param array $form_state
 */
function PVR_project_edit_project_form_submit($form, &$form_state) {
	$form_state['redirect'] = 'admin/config/PVR/projects';
}

/**
 * Callback du hook_menu
 * 
 * Display confirmation for the delete
 *  
 * @param array $form
 * @param array $form_state
 * @param string $id
 * 
 * @return drupal_confirm_form()
 */
function PVR_project_delete_project_form($form, &$form_state, $id) {
	$form['project'] = array(
		'#type' => 'value',
		'#value' => $id,
	);

	$question = t('Are you sure you want to delete the project « @id » ?', array('@id' => $id));
	$description = t('The project will be deleted');
	$path = 'admin/config/PVR/projects';
	
	$yes = t('Delete the project');
	$no = t('Return to the project list');
	return confirm_form($form, $question, $path, $description, $yes, $no);
}
/**
 * Submit de PVR_project_delete_project_form
 * 
 * @param array $form
 * @param array $form_state
 */
function PVR_project_delete_project_form_submit($form, &$form_state) {
	$result = PVR_delte_project($form['project']['#value']);
	if ($result=='1') {
		drupal_set_message(t('The project has been deleted'));
	}
	else {
		drupal_set_message(t("An error occurred"), $result);
	}
	$form_state['redirect'] = 'admin/config/PVR/projects';
}

















