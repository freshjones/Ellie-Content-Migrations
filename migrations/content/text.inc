<?php


class ContentTextMigration extends Migration 
{

    public function __construct($arguments) 
    {
        parent::__construct($arguments);
        
        $this->description = t('Import of Text Content.');
        
        $fields = array (
            'id' => 'Unique page ID',
            'text' => 'Text Field',
        );
        
        $contentDir = realpath(DRUPAL_ROOT . '/../content/pages');
        
        $contentList = new EllieContentMigrateList($contentDir, 'text');
        $contentItem = new EllieContentMigrateItem($contentDir, 'text');
        
        $this->source = new MigrateSourceList($contentList, $contentItem, $fields );
            
        $this->destination = new MigrateDestinationEntityAPI('content_elements', 'text_block');
        
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
          MigrateDestinationEntityAPI::getKeySchema('content_elements', 'text_block')
        );
        
        $this->addFieldMapping('field_body', 'text');
        
        //Unmigrate
        $unmigrate = new buildUnMigrateList();
        
        $unmigrate->setListItems(array('field_body:summary', 'field_body:format'));
        
        $unmigrate->setDefaults(array('type', 'path'));
        
        $unmigrateData = $unmigrate->getListItems();
        
        $this->addUnmigratedDestinations($unmigrateData);

    }   
    
}
