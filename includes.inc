<?php


function emberjs__node_type_exists($type) {

    $query = db_select('node_type', 't')
        ->fields('t', array('type'))
        ->condition('type', $type)
        ->execute()
        ->fetchCol();

    return (count($query) > 0) ? TRUE : FALSE;
}


function emberjs__taxonomy__get_vid_from_name($vocname) {

    $query = db_select('taxonomy_vocabulary', 'v')
        ->fields('v', array('vid'))
        ->condition('machine_name', $vocname)
        ->execute()
        ->fetchCol();

    return ($query > 0) ? $query[0] : FALSE;
}


function emberjs__taxonomy__get_by_voc_name($vid) {

    $query = db_select('taxonomy_term_data', 'd');
    $query->addField('d', 'tid', 'id');
    $query->addField('d', 'name', 'name');

    if($vid) {
        $query->condition('d.vid', $vid);
    }

    return $query->execute()->fetchAll();
}


function emberjs__messages__resource_not_found($resource) {
    return services_error(t('Could not find a resource with the name: !name', array(
        '!name'     => $resource,
    )), 404);
}

