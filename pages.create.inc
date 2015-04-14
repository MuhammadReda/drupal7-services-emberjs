<?php


module_load_include('inc', 'emberjs', 'includes');
module_load_include('inc', 'emberjs', 'includes.fields');


function emberjs__pages__v1_create_page($resource, $data) {

    $resource = check_plain($resource);

    // if found a content type with the matching resource name.
    if(emberjs__node_type_exists($resource)) {

        module_load_include('inc', 'node', 'node.pages');

        $data['type'] = $resource;
        if(!isset($data['language']))
            $data['language'] = LANGUAGE_NONE;
        if(!isset($data['uid']))
            $data['uid'] = 0;

        $formId = $resource . '_node_form';
        $form_state = array();
        $form_state['values'] = $data;
        $form_state['values']['op'] = t('Save node');
        $form_state['programmed_bypass_access_check'] = FALSE;

        $stub_node = (object) array_intersect_key($data, array_flip(array('type', 'language')));
        $stub_node->name = '';
        $stub_form = drupal_get_form($formId, (object) $stub_node);
        $form_state['triggering_element'] = $stub_form['actions']['submit'];

        drupal_form_submit($formId, $form_state, (object) $stub_node);

        if($form_errors = form_get_errors()) {
            return services_error(implode(" ", $form_errors), 406, array('form_errors' => $form_errors));
        }

        $newNid = $form_state['nid'];
        $newNode = array();
        $newNode['nid'] = $newNid;
        $newNode['uri'] = services_resource_uri(array(EMBERJS_API_BASE_URL, $resource, $newNid));

        return $newNode;
    }

    // try to find a taxonomy vocabulary with the matching resource name.
    elseif($vid = emberjs__taxonomy__get_vid_from_name($resource)) {
        return $vid;
    }

    // no matching resource, throw a 404 error.
    else {
        return services_error(t('Could not find a resource with the name: !name', array(
            '!name'     => $resource,
        )), 404);
    }

    return 'a';
}

