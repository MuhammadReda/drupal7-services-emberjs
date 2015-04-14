<?php


module_load_include('inc', 'emberjs', 'includes');
module_load_include('inc', 'emberjs', 'includes.fields');


function emberjs__pages__v1_retrieve_page($resource, $entity_id) {

    $resource = check_plain($resource);
    $entity_id = check_plain($entity_id);

    if(!$entity_id) {
        $vid = emberjs__taxonomy__get_vid_from_name($resource);
        return emberjs__taxonomy__get_by_voc_name($vid);
    }

    // if a content type with the given name exists.
    if(emberjs__node_type_exists($resource)) {
        $node = node_load($entity_id);
        if($node && $node->type === $resource) { // make sure that node exists AND node->type matches the given resource.

            // unset unwanted data
            if(isset($node->rdf_mapping)) unset($node->rdf_mapping);

            $config = emberjs__fields__get_fields_types(); // get field config
            $nodeFormatted = emberjs__fields__api_format_entity($node, $config, $resource);

            return $nodeFormatted;
        }
    }

    // if a vocabulary with the given name exists.
    if($vid = emberjs__taxonomy__get_vid_from_name($resource)) {
        $term = taxonomy_term_load($entity_id);
        if($vid == $term->vid) {

            // unset unwanted data
            if(isset($term->rdf_mapping)) unset($term->rdf_mapping);

            $config = emberjs__fields__get_fields_types(); // get field config
            $termFormatted = emberjs__fields__api_format_entity($term, $config, $resource);

            return $termFormatted;
        }
    }

    // no matching resource, throw a 404 error.
    return emberjs__messages__resource_not_found($resource);
}

