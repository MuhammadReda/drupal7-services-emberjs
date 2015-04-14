<?php



/**
 * Formats an entity.
 */
function emberjs__fields__api_format_entity($entity, $config, $resource_name, $recursive = TRUE) {

    $output = new StdClass();
    $formatted = new StdClass();

    foreach($entity as $k => $v) {
        $fieldType = isset($config[$k]) ? $config[$k] : FALSE;
        $value = emberjs__fields__get_field_value($v, $fieldType, $recursive);

        $formatted->{$k} = $value;
    }

    $output->{$resource_name} = array($formatted);

    return $output;
}



/**
 * Return field types from `field_config` table
 * as an associative array, keyed by `field_name`
 */
function emberjs__fields__get_fields_types() {

    return db_select('field_config', 't')
        ->fields('t', array('field_name', 'type'))
        ->condition('active', '1')
        ->execute()
        ->fetchAllKeyed('0');
}


/**
 *
 */
function emberjs__fields__get_field_value($field, $fieldType, $recursive) {

    $value = NULL;

    if(!is_array($field))
        return $field;

    if(count($field) == 0)
        return NULL;


    $fieldValue = reset($field);

    if(count($fieldValue) == 1) {
        return emberjs__fields__extract_field_data($fieldValue[0], $fieldType, $recursive);
    }

    $value = array();
    foreach($fieldValue as $k => $v) {
        $value[] = emberjs__fields__extract_field_data($v, $fieldType, $recursive);
    }

    return $value;
}



/**
 *
 */
function emberjs__fields__extract_field_data($field, $fieldType, $recursive) {

    $value = false;

    switch ($fieldType) {
        case 'text_with_summary':
        case 'text':
        case 'text_long':
        case 'list_boolean':
        case 'list_text':
        case 'number_decimal':
        case 'number_integer':
            $value = $field['value'];
            break;

        case 'file':
        case 'image':
            $value = file_create_url($field['uri']);
            break;

        case 'taxonomy_term_reference':
            $tid = $field['tid'];
            $term = taxonomy_term_load($tid);
            $value = new StdClass();
            $value->{$tid} = $term;
            break;

        case 'node_reference':
            $value = $field['nid'];
            break;

        default:
            $value = $field;
            break;
    }

    return $value;
}

