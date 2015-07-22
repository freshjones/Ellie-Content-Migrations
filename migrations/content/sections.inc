<?php

class EllieContentSectionMigrateList extends EllieContentMigrateList
{
}

class EllieContentSectionMigrateItem extends EllieContentMigrateItem
{

    protected function setItemObject($item)
    {
        $elementsArray = preg_split ('/\r\n|\n|\r/', $item, -1, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
        
        $contentRefs = is_array($elementsArray) && !empty($elementsArray) ? $elementsArray : NULL;
        
        $objArray = array();
        $objArray['content_ids'] = $contentRefs;
        
        $this->item = isset($objArray) && !empty($objArray) ? (object)$objArray : NULL;
        
    }

}

class ContentSectionMigration extends Migration 
{

    public function __construct($arguments) 
    {
        parent::__construct($arguments);
    
        $this->description = t('Import of Sections.');
        $this->dependencies = array
        (
          'ContentText',
        );
    
    $fields = array (
        'id' => 'Unique page ID',
        'content_ids' => 'Related Content Items',
    );
    
    $contentDir = realpath(DRUPAL_ROOT . '/../content/content');
 
    $sectionList = new EllieContentSectionMigrateList($contentDir, 'section');
    $sectionItem = new EllieContentSectionMigrateItem($contentDir, 'section');
    
    $this->source = new MigrateSourceList($sectionList, $sectionItem, $fields );
        
    $this->destination = new MigrateDestinationEntityAPI('content_elements', 'section_block');

    //Source and destination relation for rollbacks
    $this->map = new MigrateSQLMap(
      $this->machineName,
      array(
        'id' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationEntityAPI::getKeySchema('content_elements', 'section_block')
    );

    $this->addFieldMapping('field_content_element_ref', 'content_ids')
        ->sourceMigration(array('ContentText'));
    
    //Unmigrate
    $unmigrate = new buildUnMigrateList();
    
    $unmigrate->setDefaults(array('type', 'path'));
    
    $unmigrateData = $unmigrate->getListItems();
    
    $this->addUnmigratedDestinations($unmigrateData);
    
  }
}
