<?php


module_load_include('inc', 'emberjs', 'includes');
module_load_include('inc', 'emberjs', 'includes.fields');


function emberjs__pages__v1_retrieve_page($resource, $entity_id) {

    $resource = check_plain($resource);
    $entity_id = check_plain($entity_id);

    $output = 'hello there!';

    if(!$entity_id) {
        $vid = emberjs__taxonomy__get_vid_from_name($resource);
        return emberjs__taxonomy__get_by_voc_name($vid);
    }

    if(emberjs__node_type_exists($resource)) { // if a content type with the given name exists.
        $node = node_load($entity_id);
        if($node && $node->type === $resource) { // make sure that node exists AND node->type matches the given resource.

            // unset unwanted data
            unset($node->rdf_mapping);

            $config = emberjs__fields__get_fields_types(); // get field config
            $nodeFormatted = emberjs__fields__api_format_entity($node, $config);

            return $nodeFormatted;
        }
    }

    return $output;
}

