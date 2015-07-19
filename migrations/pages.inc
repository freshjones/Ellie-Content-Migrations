<?php

class EllieContentListJSON extends MigrateListJSON 
{

  protected function getIDsFromJSON(array $data)
  {

    $ids = array();

    foreach ($data as $item) 
    {
      $ids[] = $item['id'];
    }

    return $ids;

  }

}

class EllieContentItemJSON extends MigrateItemJSON 
{

  protected $data = array();

  public function getItem($id) 
  {

    // We cache the parsed JSON at $this->data.
    if (empty($this->data)) 
    {

      $data = $this->loadJSONUrl($this->itemUrl);
      
      if ($data) 
      {

        // Let's index the array by the ID for easy retrieval.
        foreach ($data as $item) 
        {
          $this->data[$item->id] = $item;
        }

      }
      else {
        // Error-handling here....
      }

    }

    // Return the requested item
    if (isset($this->data[$id])) 
    {
      return $this->data[$id];
    }
    else 
    {
      return NULL;
    }
  
  }

}

class PagesMigration extends Migration 
{

  public function __construct($arguments)
  {
     
    parent::__construct($arguments);

    $this->description = t('Page Migrations');

    $this->map = new MigrateSQLMap
    (
      $this->machineName,
      array(
        'id' => array
                ('type' => 'int',
                  'unsigned' => TRUE,
                  'not null' => TRUE,
                ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $fields = array (
      'id' => 'Unique page ID',
      'title' => 'Page title',
      'description' => 'Page body',
    );
    
    $json_file = realpath(__DIR__ . '/../data/data.json');

    $this->source = new MigrateSourceList(new EllieContentListJSON($json_file), new EllieContentItemJSON($json_file, array()), $fields);

    $this->destination = new MigrateDestinationNode('pages');
      
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'description');

    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('status')->defaultValue(1);

    /* DONT MIGRATE */
        
    //Unmigrate        
    $unmigrate = new buildUnMigrateList();

    $unmigrate->setListItems(array('body:format','body:summary'));
    $unmigrate->setListItems(array('path'));

    $unmigrate->setListItems(array('metatag_title','metatag_description','metatag_abstract', 'metatag_keywords'));
    $unmigrate->setListItems(array('metatag_robots','metatag_news_keywords','metatag_standout', 'metatag_rating'));
    $unmigrate->setListItems(array('metatag_referrer','metatag_generator','metatag_rights', 'metatag_image_src'));
    $unmigrate->setListItems(array('metatag_canonical','metatag_shortlink','metatag_publisher', 'metatag_author'));
    $unmigrate->setListItems(array('metatag_original-source','metatag_prev','metatag_next', 'metatag_revisit-after'));
    $unmigrate->setListItems(array('metatag_content-language','metatag_geo.position','metatag_geo.placename', ));
    $unmigrate->setListItems(array('metatag_geo.region', 'metatag_icbm', 'metatag_refresh'));

    $unmigrateData = $unmigrate->getListItems();
        
    $this->addUnmigratedDestinations($unmigrateData);

  }

}
