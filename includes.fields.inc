<?php



/**
 * Formats an entity.
 */
function emberjs__fields__api_format_entity($entity, $config, $resource_name, $recursive = TRUE) {

    $output = new StdClass();
    $formatted = new StdClass();

    foreach($entity as $k => $v) {
        $fieldType = isset($config[$k]) ? $config[$k] : FALSE;
        $value = emberjs__fields__get_field_value($v, $fieldType, $config, $recursive);

        if(!isset($value['has_attachment'])) {
            $formatted->{$k} = $value;
        }
        else {
            $formatted->{$k} = $value['value'];

            $bundle = $value['bundle'];
            $attachment = $value['attachment'];

            if($bundle === $resource_name) // avoid collision with original resource
                $bundle .= '_1';

            if(is_array($attachment)) {
                foreach($attachment as $a => $b) {
                    $output->{$bundle}[] = $b;
                }
            }
            else {
                $output->{$bundle}[] = $attachment;
            }
        }
    }

    $output->{$resource_name} = array($formatted);

    return $recursive ? $output : $formatted;
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
function emberjs__fields__get_field_value($field, $fieldType, $config, $recursive) {

    $value = NULL;

    if(!is_array($field))
        return $field;

    if(count($field) == 0)
        return NULL;


    $fieldValue = reset($field);

    if(count($fieldValue) == 1) {
        return emberjs__fields__extract_field_data($fieldValue[0], $fieldType, $config, $recursive);
    }

    $value = array();
    foreach($fieldValue as $k => $v) {
        $temp = emberjs__fields__extract_field_data($v, $fieldType, $config, $recursive);

        if(!isset($temp['has_attachment'])) {
            $value[] = $temp;
        }
        else {
            $value['has_attachment'] = TRUE;
            $value['bundle'] = $temp['bundle'];
            $value['value'][] = $temp['value'];
            $value['attachment'][] = $temp['attachment'];
        }
    }

    return $value;
}



/**
 *
 */
function emberjs__fields__extract_field_data($field, $fieldType, $config, $recursive) {

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
            if($recursive) {
                $term = taxonomy_term_load($tid);
                if($term) {
                    $bundle = $term->vocabulary_machine_name;
                    $term = emberjs__fields__api_format_entity($term, $config, $bundle, FALSE);
                    $value = array(
                        'has_attachment'    => TRUE,
                        'value'             => $tid,
                        'bundle'            => $bundle,
                        'attachment'        => $term,
                    );
                }
                else {
                    $value = $tid;
                }
            }
            else {
                $value = $tid;
            }
            break;

        case 'node_reference':
            $nid = $field['nid'];
            if($recursive) {
                $node = node_load($nid);
                if($node) {
                    $bundle = $node->type;
                    $node = emberjs__fields__api_format_entity($node, $config, $bundle, FALSE);
                    $value = array(
                        'has_attachment'    => TRUE,
                        'value'             => $nide,
                        'bundle'            => $bundle,
                        'attachment'        => $node,
                    );
                }
                else {
                    $value = $nid;
                }
            }
            else {
                $value = $nid;
            }
            break;

        default:
            $value = $field;
            break;
    }

    return $value;
}

